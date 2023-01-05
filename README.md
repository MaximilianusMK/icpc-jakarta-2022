# ICPC Jakarta 2022 - Problem Repository

## Repository Structure

Each problem (from PA to PD for trial and A to M for contest) has their own folder. The folder name is the `slug` of the problem, which is the codename we used when we internally prepare the problems. The following table describes the slug for each problem

| Problem Code | Problem Slug | Problem Title          |
| ------------ | ------------ | -----------------------|
| PA           | double       | Non Classical Problem  |
| PB           | uniform      | Uniform Maker          |
| PC           | maxdiff      | Maxdifficient Group    |
| PD           | cell         | Cell Game              |
| A            | eggs         | Storing Eggs           |
| B            | barrier      | Magical Barrier        |
| C            | memory       | Nightmare Brother      |
| D            | city         | City Hall              |
| E            | sort         | Substring Sort         |
| F            | gcd          | Doubled GCD            |
| G            | mode         | The Only Mode          |
| H            | grid         | Grid Game              |
| I            | plan         | Contingency Plan       |
| J            | bread        | Sharing Bread          |
| K            | function     | Short Function         |
| L            | toll         | Increase the Toll Fees |
| M            | flawed       | Game Show              |

In the problem folder, there should be exactly the following file/folder:

- `description.pdf`. The problem statement distributed to the contestants during the contest.
- `data/`. A folder consisting of all testcases. The sample inputs given in the problem statement will be in the form of `sample/inc-<slug>_sample_<testcase number>.in/ans`, while the hidden testcases will be in the form of `secret/inc-<slug>_1_<testcase number>.in/ans`.
- `solution.cpp`. The official solution used to generate the outputs of the testcases
- `scorer.cpp`. A validator used to validate contestants' output. The code accepts three arguments.
  - The first parameter will be used as the filename of the input file
  - The second parameter will be used as the filename of the judge's output file
  - The third parameter will be used as the filename of the contestant's output file
  - This code will print `WA` to stdout if the contestant's output is intended to be judged `WRONG ANSWER`, while it will print `AC` to stdout if the contestants's output is intended to be judged `CORRECT`.
- `verifier.py`. A verifier used to validate the input of the testcases. The code will crash if the input violates the problem constraint, while it will exit gracefully and will not output anything if the input satisfies the problem constraint.

## Limitations

The following table describes the time limit for each problem, as broadcasted to all teams at the beginning of the contest:

| Problem Code | Problem Slug | Time Limit |
| ------------ | ------------ | ---------- |
| PA           | double       | 1 sec      |
| PB           | uniform      | 1 sec      |
| PC           | maxdiff      | 1 sec      |
| PD           | cell         | 2 sec      |
| A            | eggs         | 1 sec      |
| B            | barrier      | 1 sec      |
| C            | memory       | 1 sec      |
| D            | city         | 1 sec      |
| E            | sort         | 4 sec      |
| F            | gcd          | 1 sec      |
| G            | mode         | 3 sec      |
| H            | grid         | 1 sec      |
| I            | plan         | 1 sec      |
| J            | bread        | 3 sec      |
| K            | function     | 1 sec      |
| L            | toll         | 1 sec      |
| M            | flawed       | 1 sec      |

The memory limit for all problems is 256MB, as broadcasted to all teams at the beginning of the contest.

## Solutions

Note that while there is only one official published solution, we have a lot of other solutions prepared for our internal testing. These other solutions include, but not limited to:

- Solutions in several other languages
- Solutions that is intended to be incorrect (e.g. WA, TLE, etc.)
- Other correct solutions (especially on problems with more than one correct outputs for each input)

## Errata

None.
