#!/bin/bash

# Get the current branch name
current_branch=$(git rev-parse --abbrev-ref HEAD)

# Find the most recent commit where the branch diverged from
merge_base=$(git merge-base "$current_branch" "$(git for-each-ref --format='%(refname:short)' refs/heads | grep -v "^$current_branch$" | xargs -I % git merge-base "$current_branch" % | sort | uniq -c | sort -nr | head -1 | awk '{print $2}')")

# Find the branch that contains the merge base commit
parent_branch=$(git branch --contains "$merge_base" | grep -v "$current_branch" | sed -e 's/^[ \t]*//' | head -1)

# If no parent branch is found, handle it gracefully
if [ -z "$parent_branch" ]; 
then
  echo "Could not determine the parent branch for the current branch: $current_branch"
else
  echo "Current branch: $current_branch"
  echo "Parent branch: $parent_branch"
fi

