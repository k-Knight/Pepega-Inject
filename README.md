# Pepega-Inject

An injector for Discord that allows one to inject CSS and JavaScript.

This injector is so **simple** that it does not require the modification of discord '*.asar*' application archive

## Installation

The prerequisite for using installer is that you'll have to install Python scripting language. You can the newest version from here: [Download Python](https://www.python.org/downloads/). For now I recommend using ``3.7.5`` version of Python.

Just run the **``isntaller.pyw``** and select the Discord's main application folder and the injector will be installed. Alternatively, you can run the **``isntaller.py``** if you are confident that installation will complete correctly without your input.

## Developing plugins

The developing of plugins is super basic, just add a folder with the name of your plugin in the ``pepega-inject/plugins`` folder and drop some JavaScript and CSS files in there.

### Adding settings to your plugin

Pepega-Inject allows your plugin to have settings in JSON file format. The settings need to be concentrated in a single file called **``settings.json``** and put somewhere within the your plugin folder.

The plugin settings can be accessed by plugin through the following object: ``PepegaInject.settings``. Example code:
```JavaScript
// gets value of the specified attribute 
PepegaInject.settings["some-attribute"]
```
### Loaded functionality

The plugin loader of the Pepega-Inject injects each plugin with additional functionality that can be used anywhere withing the plugin. The loaded functionality can be accessed through the ``PepegaInject`` object.
<table>
<tr>
<td>

**Object**

</td>
<td>

**Description**

</td>
</tr>
<tr>
<td>

**``IPC``**

</td>
<td>

Allows to execute **NodeJS** code in the main Electron environment process ([Electron documentation](https://www.electronjs.org/docs/api/ipc-renderer)) through the ``execute ( string, function)`` function with JavaScript code and callback function as arguments.

Allows to send data back to the render process from withing the executable code through the ``pepegaSendValue( object )`` function.

Example:
```JavaScript
PepegaInject.IPC.execute(`
    try {
        pepegaSendValue('Hello World');
    }
    catch (err) {
        console.error(err);
    }
`);
```

</td>
</tr>
</table>