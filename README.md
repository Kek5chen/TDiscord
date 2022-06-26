# TDiscord

TDiscord is an implementation of the Discord API which can be used to automate bots and user accounts.

# Setup and Usage

The main.cpp file contains a basic setup and usage example. The example is written in C++ and uses the TDiscord library.
It is setup to run on a token which is supplied in `the startup arguments`.

# Receiving your token

You can do this by going to the official Discord website and [creating an application](https://discordapp.com/developers/applications/me). Then, go to the `OAuth` tab and enter the `Bot` section. You can then use the token there to run the example.

# Expanding on the example

The application provides connectivity over the Discord WebSocket Gateway and the REST API. You can use the REST API to send messages, set your status and more.
The gateway is used to receive events and messages which you can then act on.

# Building with Visual Studio

Clone the repository and build the project.

1. Clone the repository
    ```bash
    git clone https://github.com/Kek5chen/TDiscord TDiscord
    ```
2. Open the repository in Visual Studio
3. Open the `main.cpp` file in Visual Studio
4. Build the project by clicking on the `Build` button near the top.