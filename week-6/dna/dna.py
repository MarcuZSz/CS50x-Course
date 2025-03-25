import csv
import sys


def main():
    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Read database file into a variable
    database = []
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        database = list(reader)
        # Get STR names from header
        str_names = list(database[0].keys())[1:]

    # Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as file:
        sequence = file.read()

    # Find longest match of each STR in DNA sequence
    str_counts = {}
    for str_name in str_names:
        str_counts[str_name] = longest_match(sequence, str_name)

    # Check database for matching profiles
    for person in database:
        matches = 0
        for str_name in str_names:
            if int(person[str_name]) == str_counts[str_name]:
                matches += 1
        
        # If all STRs match, we found our person
        if matches == len(str_names):
            print(person["name"])
            return

    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            
            # If there is no match in the substring
            else:
                break
        
        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
