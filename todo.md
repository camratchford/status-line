## ANSI to define:

We should allow users to customize the colours.

- `WHITE='\[$(tput setaf 15)\]'`
- `BLUE='\[$(tput setaf 6)\]'`
- `BEIGE='\[$(tput setaf 221)\]'`
- `GREEN='\[$(tput setaf 35)\]'`
- `RED='\[$(tput setaf 124)\]'`
- `LIGHT_RED='\[$(tput setaf 203)\]'`
- `PURPLE='\[$(tput setaf 140)\]'`
- `BROWN='\[$(tput setaf 248)\]'`
- `RESET='\[$(tput sgr0)\]'`
- `UP_ARROW='↑'`
- `DOWN_ARROW='↓'`
- `UP_DOUBLE_ARROW='⇑'`
- `DOWN_DOUBLE_ARROW='⇓'`


## Git data to figure out:
- [x] Determine if we're in a git repo: `git rev-parse --is-inside-work-tree`
- [] Determine file status: `git status --short`
  - Write the symbol and the number of items which match the below in red: 
    - Untracked: `?`
    - Added:     `+`
    - Deleted:   `-`
    - Modified:  `~`
    - Others will be given, but we can ignore as that would be too verbose
  - If no changes were detected, instead write a `✓` in green
- Determine remote name: `git remote`
- Determine parent branch: `git symbolic-ref refs/remotes/${remote_name}/HEAD --short`
- Determine current branch: `git rev-parse --abbrev-ref HEAD`
- Check if your local default branch is ahead or behind: `git rev-list --left-right --count ${remote_name}/${parent_branch}...HEAD`
  - 0 = Ahead:  `↑` (in teal)
  - 1 = Behind: `↓` (in teal)
- Check if your current branch is ahead or behind the default: `git rev-list --left-right --count @{u}...HEAD`
  - 0 = Ahead:  `⇑` (in purple)
  - 1 = Behind: `⇓` (in purple)

## Export some environment variables

Terminal title: 
- Get username
- Get PWD
```c++
    printf("\033]0;%s - %s\a", username, pwd);
```

