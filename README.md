### Tequila Sunlight

This tool is used by Fishing Cactus to get more visiblity on build results.

The main purpose is to connect a traffic light on a build server and use light combinations to show build results.

The devices are :

    Microchip PIC32MX4-80
    Bluetooth DIP Module WRL-08550
    A build server with Hudson
    A traffic light 

The software parts in this project are :

    An application to control the microchip running on StickOS
    A Hudson plugin that analyzes build results and send http requests to a daemon
    A daemon that receives http requests and send commands to the microchip through bluetooth
    