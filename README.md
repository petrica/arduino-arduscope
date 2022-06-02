# ArduScope

## Usage

Open up the [client application](https://petrica.github.io/arduino-arduscope/)

## Getting started

If you are in a possession of an Arduino Uno board, you should be able to start analysing signals in just a few minutes by following this guide.
1. Burn the <a href="https://create.arduino.cc/editor/petrica/9526d9c0-3c3b-4b43-b31f-138ef5fb6f5e/preview" target="_blank">Arduscope firmware</a> to your Arudino Uno board.

2. <a href="https://create.arduino.cc/getting-started/plugin/welcome" target="_blank">Install Arduino Create Agent</a>. A piece of software that handles the serial communication between the Arduino board and the Arduscope web interface. <a href="https://github.com/arduino/arduino-create-agent" target="_blank">Github sources here</a>. 

![Arduscope connection diagram](/docs/arduino-agent-diagram.png)

3. Configure Arduino Create Agent to accept requests from <a href="https://petrica.github.io/arduino-arduscope/" target="_blank">Arduscope web interface</a>. You will have to find the installation folder for ``arduino-create-agent``. The location of this folder depends on your operating system:

    * __MacOS__: ``/Users/[your-username]/Applications/ArduinoCreateAgent/ArduinoCreateAgent.app/Contents/MacOS/Arduino_Create_Agent``

    Under this folder you should see a ``config.ini`` file that is the main configuration file used by the Arduino Web IDE.<br/>
    To make the Arduino Agent accept connection from our web interface, create a new configuration file ``arduscope.ini``, and paste the following lines:

    ```ini
    gc = std
    name = arduscope
    origins = https://petrica.github.io
    crashreport = true
    ```

    This will enable CORS for the URL ``https://petrica.github.io``, where the Arduscope web interface sits.

    * Restart Arduino Agent. You should find the agent in your tray bar.
    * From the dropdown menu select our new configuration profile:<br/>
    ![Arduscope - Config Menu](/images/arduino/arduscope-agent-menu.png)

4. Access the <a href="https://petrica.github.io/arduino-arduscope/" target="_blank">Arduscope web interface</a> and connect your Arduino Uno to the USB. 
You should see the USB port displayed in the Arduscope web interface. 
Click ``Connect`` and once successful the Sample Rate dropdown should be populated with the sampling frequencies that the Arduscope is capable of handling.<br/>
![Arduscope - Web Port](/docs/arduscope-web-port.png)
5. Connect pin ``A0`` to pin ``11`` on the Arduino board and click ``Acquire`` on the Arduscope web interface. You should see the 500Hz calibration signal displayed.<br/>
![Arduscope - Calibrator](/docs/arduscope-calibrator.png)