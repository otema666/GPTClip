# GPTCLip


### Overview

This application allows you to use ChatGPT discreetly. It works by copying text to the clipboard, then pressing **Ctrl + Alt + 0**, which sends a request to a server that has an API for ChatGPT. The response is retrieved from the server and automatically pasted back into the clipboard. This seamless flow makes interacting with ChatGPT quick and invisible, allowing for an efficient workflow without opening additional windows or tabs.

### Key Features
1. **Clipboard Monitoring**: The app monitors the clipboard and captures the copied text to be sent as a prompt to ChatGPT.
2. **Hotkey Activation**: Press **Ctrl + Alt + 0** to trigger the request to ChatGPT, instantly sending the clipboard content for processing.
3. **Instant Response**: The ChatGPT API responds with the requested information, and the app pastes the result back into the clipboard.
4. **Graphical User Interface (GUI)**: The app provides a clean and simple interface with options for:
   - Starting the server or service.
   - Exiting the application.
   - Viewing help information.
   - Selecting the type of response (e.g., multiple-choice or long-form answers).
   - Enabling or disabling notifications.


### Graphical User Interface (GUI)

The GUI starts by asking the user to log in, ensuring secure access to the application's features. Once logged in, the interface provides the following components:

- **Welcome Message**: Displays a personalized greeting with the logged-in username.
- **Send Notifications**: A checkbox that allows the user to choose whether to receive notifications.
- **Keyboard Shortcut Display**: Shows the shortcut (**Ctrl + Alt + 0**) to trigger the API request.
- **Response Type Selection**: A dropdown menu that allows users to select the type of responses they want from ChatGPT:
   - Multiple-choice exam style.
   - Long-form answers (essay style).
- **Buttons**:
   - **Start**: To initiate the background service.
   - **Exit**: To close the application.
   - **Help**: To provide user assistance and instructions.


### API Integration

This application leverages an API that is possible thanks to the repository at [Duck Chat](https://github.com/mrgick/duck_chat). A huge thanks goes out to mrgick, the creator of this repository for making it possible to integrate the ChatGPT functionality. The API used is written in Python and handles the communication with ChatGPT through the server.

#### Login Process
The login functionality is managed by making a request to a PHP script hosted on the server. This script checks the user's credentials against a database. The database setup is included in the repository and can be found in the `sql/database.sql` file. This makes it easy to manage user authentication and data securely.

### Windows-Only Compatibility

Please note that this application is specifically designed and tested for **Windows** systems. All functionalities and integrations are optimized for this platform, ensuring that the app runs smoothly in a Windows environment.
