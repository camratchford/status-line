# status-line

> A Git status `$PS1` prompt generator to be used with the `PROMPT_COMMAND` shell variable.

## About




## Build and Install

### Requirements

- make
- cmake >= 3.28
- gcc >= 12
- libgit2-dev

For Ubuntu, install:

```
sudo apt-get install make cmake gcc-12 libgit2-dev
```

### Build

```bash
git clone https://github.com/camratchford/status-line ~/status-line
cd ~/status-line
cmake -B build -S . && make -C build
```

### Install

```bash
# Put it somewhere in the user's PATH variable, `~/.local/bin` for me.
cp ./build/status-line ${SOMEWHERE_IN_USERS_PATH}/status-line
# Add the `PROMPT_COMMAND` variable to `~/.bashrc` or other similar file.
echo "PROMPT_COMMAND='export PS1=\$(status-line)'" >> ~/.bashrc
# reload bash
exec bash
```
