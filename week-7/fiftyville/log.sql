-- Keep a log of any SQL queries you execute as you solve the mystery.

.mode column
.headers on

-- Looking at the tables with .tables
/*
airports              crime_scene_reports   people              
atm_transactions      flights               phone_calls         
bakery_security_logs  interviews          
bank_accounts         passengers 
*/

-- Checking crime_scene_reports
SELECT * FROM crime_scene_reports WHERE street = "Humphrey Street" AND year = 2024 AND month = 07 AND day = 28;

/*
id   year  month  day  street           description                                                                                                                                                                                                             
---  ----  -----  ---  ---------------  ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
295  2024  7      28   Humphrey Street  Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
297  2024  7      28   Humphrey Street  Littering took place at 16:36. No known witnesses.         
*/

-- Check the interviews table
SELECT * FROM interviews WHERE month >= "07" AND day >= "28";

/*
id   name     year  month  day  transcript                                                                                                                                                                                                                                                                                                         
---  -------  ----  -----  ---  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
158  Jose     2024  7      28   “Ah,” said he, “I forgot that I had not seen you for some weeks. It is a little souvenir from the King of Bohemia in return for my assistance in the case of the Irene Adler papers.”                                                                                                                              
159  Eugene   2024  7      28   “I suppose,” said Holmes, “that when Mr. Windibank came back from France he was very annoyed at your having gone to the ball.”                                                                                                                                                                                     
160  Barbara  2024  7      28   “You had my note?” he asked with a deep harsh voice and a strongly marked German accent. “I told you that I would call.” He looked from one to the other of us, as if uncertain which to address.                                                                                                                  
161  Ruth     2024  7      28   Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                         
162  Eugene   2024  7      28   I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                
163  Raymond  2024  7      28   As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
191  Lily     2024  7      28   Our neighboring courthouse has a very annoying rooster that crows loudly at 6am every day. My sons Robert and Patrick took the rooster to a city far, far away, so it may never bother us again. My sons have successfully arrived in Paris.  
*/

-- Checking security logs of the bakery
SELECT license_plate FROM bakery_security_logs 
WHERE year = 2024 AND month = 7 AND day = 28 
AND hour = 10 AND minute BETWEEN 15 AND 25 
AND activity = 'exit';

/*
license_plate
-------------
5P2BI95      
94KL13X      
6P58WS2      
4328GD8      
G412CB7      
L93JTIZ      
322W7JE      
0NTHK55 
*/

-- Looking at the ATM withdrawals at Leggett Street
SELECT account_number FROM atm_transactions 
WHERE year = 2024 AND month = 7 AND day = 28 
AND atm_location = 'Leggett Street' 
AND transaction_type = 'withdraw';

/*
account_number
--------------
28500762      
28296815      
76054385      
49610011      
16153065      
25506511      
81061156      
26013199  
*/

-- Trying to identify suspect with these infos
SELECT * FROM people
WHERE id IN (
SELECT person_id FROM bank_accounts 
WHERE account_number IN (SELECT account_number FROM atm_transactions 
WHERE year = 2024 AND month = 7 AND day = 28 
AND atm_location = 'Leggett Street' 
AND transaction_type = 'withdraw')
)
AND license_plate IN (SELECT license_plate FROM bakery_security_logs 
WHERE year = 2024 AND month = 7 AND day = 28 
AND hour = 10 AND minute BETWEEN 15 AND 25 
AND activity = 'exit')
AND phone_number IN (SELECT caller FROM phone_calls 
WHERE year = 2024 AND month = 7 AND day = 28 
AND duration < 60);

/*
id      name   phone_number    passport_number  license_plate
------  -----  --------------  ---------------  -------------
514354  Diana  (770) 555-1861  3592750733       322W7JE      
686048  Bruce  (367) 555-5533  5773159633       94KL13X  
*/

-- So either Diana or Bruce is the suspect in question

-- Now checking the flights on the 29th
SELECT id, destination_airport_id FROM flights 
WHERE origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville') 
AND year = 2024 AND month = 7 AND day = 29 
ORDER BY hour, minute 
LIMIT 1;

/*
id  destination_airport_id
--  ----------------------
36  4                     
*/

-- Checking the passengers on that flight
SELECT passport_number FROM passengers 
WHERE flight_id = (SELECT id FROM flights 
WHERE origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville') 
AND year = 2024 AND month = 7 AND day = 29 
ORDER BY hour, minute LIMIT 1);

/*
passport_number
---------------
7214083635     
1695452385     
5773159633     
1540955065     
8294398571     
1988161715     
9878712108     
8496433585   
*/

-- Checked the passport_numbers I can see that Bruce was on that plane

-- Identifying his accomplice via phone call logs
SELECT name FROM people 
WHERE phone_number = (SELECT receiver FROM phone_calls 
WHERE caller = (SELECT phone_number FROM people WHERE name = 'Bruce') 
AND year = 2024 AND month = 7 AND day = 28 
AND duration < 60);

/*
name 
-----
Robin
*/

-- So a man named Robin is Bruce's accomplice

-- Last task to check his destination
SELECT city FROM airports 
WHERE id = (SELECT destination_airport_id FROM flights 
WHERE year = 2024 AND month = 7 AND day = 29 
ORDER BY hour, minute 
LIMIT 1);

/*
city         
-------------
New York City
*/

/*After a lot of trying and scrapping data finally found a fast and efficient way to find the answer omg*/