# Prevent paste event while it is scrolling with the TrackPoint Keyboard II USB

The TrackPoint lovers would understand the annoying behavior of this keyboard when you want to scroll using the 
TrackPoint just like any ThinkPad TrackPoint laptop.

## Requirements

- xdotool
- xinput

## Installation

We need to disable the paste event of the built-in mouse and finally listen its
events in order to be able triggering the paste event properly

```
xinput set-button-map 'Lenovo TrackPoint Keyboard II Mouse' 1 10 3 4 5 6 7
xinput test 'Lenovo TrackPoint Keyboard II Mouse' | tpscrollfix
```

### Notes
- You will need to build the binary  **tpscrollfix**
- You need to move the binary to `/usr/bin` or where you prefer
