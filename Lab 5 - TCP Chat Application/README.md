# Lab 5: TCP Chat Application

## Run in your Codespace terminal

This is a client/server pair, so it needs **two terminal tabs**: start
the server first, then run the client against it. To open a second
tab, click the `+` in the terminal panel, or use `Terminal -> New
Terminal`.

Each block below is self-contained and copy-paste safe from any
directory in the Codespace: it `cd`s straight to this folder and
restores the executable bit before running (the bit does not survive a
fresh clone, so this is always safe to include). These use the
standard Codespaces path, `/workspaces/Computer-Networks`; if you
cloned elsewhere, swap in your own path.

Terminal 1:

```bash
cd "/workspaces/Computer-Networks/Lab 5 - TCP Chat Application"
chmod +x run_*
./run_5.1_chat_server
```

Terminal 2:

```bash
cd "/workspaces/Computer-Networks/Lab 5 - TCP Chat Application"
chmod +x run_*
./run_5.1_chat_client
```

Once connected, the client sends a line, then the server operator
types a reply, back and forth. Type `logout` on either side to end the
chat: a client-sent `logout` ends the session immediately without a
reply; a server-typed `logout` is sent to the client first so it can
print its own "Server logged out" message before exiting.

### The compact versions

```bash
cd "/workspaces/Computer-Networks/Lab 5 - TCP Chat Application/compact"
chmod +x run_*
./run_5.1_chat_server
```

```bash
cd "/workspaces/Computer-Networks/Lab 5 - TCP Chat Application/compact"
chmod +x run_*
./run_5.1_chat_client
```

## Full vs Compact

**Full** version is commented for understanding: it explains *why*
each choice works, not just *what* the code does.

**Compact** version in `compact/` is the shortest code that still
compiles clean under `-Wall -Wextra -Wpedantic` and produces the same
output. It has its own `run_*` wrapper, so it stands on its own.
