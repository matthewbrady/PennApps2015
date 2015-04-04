# ShakeShock
A Pebble SmartWatch app written during PennApps winter 2015. 

Team members: Matthew Brady, Sumedh Guha, Ethan Weinberger, and Yuatro Yamada

# Purpose

Ever had that one class you just can't keep your eyes open for? While we can't make your lecture more exciting, we can help keep you awake. Running on your Pebble smart watch, ShakeShock will keep track of your movement duing lecture, detect when you begin to fall asleep, and silently deliver vibration to your wrist to wake you up and keep you alert. 

# Features
- Silent vibration-based alarm wakes you up without disturbing those around you
- On-screen alerts and a wake-up countdown prompt you to avoid false-positives
- Scheduling: Input your schedule, and the app will instruct your watch to run ShakeShock automatically at the begining of every class


# Technology

In order to assess the alertness of the subject, ShakeShock monitors watch accelerometer data over time and applies a simple formula: how much has the subject moved in the last 30 seconds? Our field tests demonstrate that students taking notes almost always make some detectable movement over 30 seconds (say, flipping a page or adjusting their paper), even when the watch is on their non-dominant wrist. To this end, we run acclerometer data through a filter to see if the changes exceed a certain threshold for noise, and trigger an alert as soon as significant time has passed without movement.

Video demonstration: https://www.youtube.com/watch?v=ioQout2L_YE
