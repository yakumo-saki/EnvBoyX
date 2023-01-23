# how to develop

## arduino code

1. git clone
2. open directory in VSCode
3. Fix includePath

### include path

Edit `.vscode/c_cpp_properties.json` file.

```
"includePath": [
    "${workspaceFolder}/**",
    "/Users/username/.platformio/packages/**"  <-- Add this. path vary on your OS.
],
```

## Web UI code

1. git clone
2. edit `src/global.cpp` `DEBUG_BUILD = true`. by change this, envboy returns CORS header.
3. Upload to board
4. exec `python -m http.server` In `embed` directory
5. access `localhost:8000` using web browser

### NOTE

1. you cant use external JS libraries. because host has no internet access while in setup mode.

NOTE: modernize JS/Web development is a task in near future.