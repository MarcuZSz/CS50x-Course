import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get user's stocks and shares
    stocks = db.execute("""
        SELECT symbol, SUM(shares) as total_shares
        FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        HAVING total_shares > 0
    """, session["user_id"])
    
    # Get current price for each stock
    total_value = 0
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["price"] = quote["price"]
        stock["value"] = stock["price"] * stock["total_shares"]
        total_value += stock["value"]
    
    # Get user's cash balance
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    total_value += cash
    
    return render_template("index.html", stocks=stocks, cash=cash, total=total_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    
    # Get form data
    symbol = request.form.get("symbol")
    shares = request.form.get("shares")
    
    # Validate input
    if not symbol:
        return apology("must provide symbol")
    if not shares or not shares.isdigit() or int(shares) <= 0:
        return apology("must provide positive number of shares")
    
    # Look up stock info
    quote = lookup(symbol)
    if quote is None:
        return apology("invalid symbol")
    
    # Calculate total cost
    price = quote["price"]
    total_cost = price * int(shares)
    
    # Check user's cash balance
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    if total_cost > user_cash:
        return apology("can't afford")
    
    # Update database
    db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", total_cost, session["user_id"])
    db.execute("""
        INSERT INTO transactions (user_id, symbol, shares, price, type)
        VALUES (?, ?, ?, ?, 'buy')
    """, session["user_id"], symbol.upper(), int(shares), price)
    
    flash("Bought!")
    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Get all transactions for the user
    transactions = db.execute("""
        SELECT symbol, shares, price, type, timestamp 
        FROM transactions 
        WHERE user_id = ? 
        ORDER BY timestamp DESC
    """, session["user_id"])
    
    # Calculate the total price for each transaction
    for transaction in transactions:
        transaction["total"] = transaction["shares"] * transaction["price"]
    
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    
    symbol = request.form.get("symbol")
    if not symbol:
        return apology("must provide symbol")
    
    quote = lookup(symbol)
    if quote is None:
        return apology("invalid symbol")
    
    return render_template("quoted.html", quote=quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    
    # Access form data
    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    # Validate input
    if not username:
        return apology("must provide username")
    elif not password:
        return apology("must provide password")
    elif not confirmation:
        return apology("must confirm password")
    elif password != confirmation:
        return apology("passwords do not match")
    
    try:
        # Insert new user into database
        password_hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, password_hash)
        return redirect("/login")
    except:
        return apology("username already exists")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Get user's stocks for the dropdown menu
    stocks = db.execute("""
        SELECT symbol, SUM(shares) as total_shares
        FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        HAVING total_shares > 0
    """, session["user_id"])

    if request.method == "GET":
        return render_template("sell.html", stocks=stocks)

    # Get form data
    symbol = request.form.get("symbol")
    shares = request.form.get("shares")

    # Validate input
    if not symbol:
        return apology("must provide symbol")
    if not shares or not shares.isdigit() or int(shares) <= 0:
        return apology("must provide positive number of shares")

    shares = int(shares)

    # Check if user owns enough shares
    for stock in stocks:
        if stock["symbol"] == symbol:
            if shares > stock["total_shares"]:
                return apology("too many shares")
            break
    else:
        return apology("symbol not found in portfolio")

    # Get current stock price
    quote = lookup(symbol)
    if quote is None:
        return apology("invalid symbol")

    # Calculate total value of sold shares
    price = quote["price"]
    total_value = price * shares

    # Update database
    db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", 
               total_value, session["user_id"])
    db.execute("""
        INSERT INTO transactions (user_id, symbol, shares, price, type)
        VALUES (?, ?, ?, ?, 'sell')
    """, session["user_id"], symbol.upper(), -shares, price)

    flash("Sold!")
    return redirect("/")
