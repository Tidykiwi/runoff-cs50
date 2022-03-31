#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

int i;
int j;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    i = voter;
    j = rank;

    for (int x = 0; x < candidate_count; x++)
    {
        if (strcmp(name, candidates[x].name) == 0)
        {
            preferences[i][j] = x;
            return true;
        }

    }

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int x = 0; x < voter_count; x++)
    {
        for (int y = 0; y < candidate_count; y++) // i think/hope this loop will continue to switch to the next preference if the...
        {
            if (candidates[preferences[x][y]].eliminated == true)
            {
                printf("");
            }
            else if (candidates[preferences[x][y]].eliminated == false)
            {
                candidates[preferences[x][y]].votes++;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    for (int x = 0; x < candidate_count; x++)
    {
        int v = (voter_count / 2) + 1;
        if (candidates[x].votes == v)
        {
            string winner = candidates[x].name;
            printf("%s\n", winner);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int f = 0;
    int t = 0;
    int min;

    for (int x = 0; x < candidate_count; x++)
    {
        if (candidates[x].eliminated == true)
        {
            break;
        }
        if (candidates[x].eliminated == false)
            for (int y = 0; y < candidate_count; y++)
            {
                if (candidates[y].eliminated == false)
                {
                    if (candidates[x].votes < candidates[y].votes)
                    {
                        f++;
                    }
                    else if (candidates[x].votes == candidates[y].votes)
                    {
                        f++;
                    }
                }
                else if (candidates[y].eliminated == true)
                {
                    t++;
                }
            }

        if (f + t == candidate_count)
        {
            min = candidates[x].votes;
        }
        f = 0;
        t = 0;

    }


    return min;

}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int v = 0;
    int e = 0;

    for (int x = 0; x < candidate_count; x++)
    {
        if (candidates[x].eliminated == true)
        {
            e++;
        }
        if (candidates[x].eliminated == false && candidates[x].votes == min)
        {
            v++;
        }
    }
    if (v + e == candidate_count)
    {
        return true;
    }
    v = 0;
    e = 0;

    return false;

}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    for (int x = 0; x < candidate_count; x++)
    {
        if (candidates[x].votes == min)
        {
            if (candidates[x].eliminated == false)
            {
                candidates[x].eliminated = true;
            }
        }
    }
    return;
}
