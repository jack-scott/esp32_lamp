

Add this to your `.vscode/launch.json` so that you can debug your html in the browser
```
{
    "version": "0.2.0",
    "configurations": [
        {
            "type": "chrome",
            "request": "launch",
            "name": "Launch indexhtml",
            "file": "${workspaceFolder}/espLamp/include/index.html"
        }
          
    ]
}
```