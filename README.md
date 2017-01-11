Team Foundation Server plugin for Qt Creator
============================================

[![Build status](https://ci.appveyor.com/api/projects/status/legfgybaww8qlinq?svg=true)](https://ci.appveyor.com/project/jesperhh/teamfoundation)


Requirements
------------
Either
* Team Explorer (only tested with 2012 edition)
  - http://www.microsoft.com/en-us/download/details.aspx?id=30656
Or
* Visual Studio (tested with 2012 and 2013 edition)

Some basic functionality works with Team Explorer Everywhere, but it does not have the GUI components that the regular tf.exe has.

Features
----------------
* Import project from Team Foundation Server
* Add/Delete/Undo/Get latest
* Compare/Annoate/History/Check in
  - These work a little differently than other source control plugins for Qt Creator, because they reuse the GUI offered by tf.exe.

Author
-------
* Jesper Hellesø Hansen (http://oktet.net, jesper@oktet.net)

Installation
------------
* Either compile from source (you will need to compile Qt Creator from source and modify TeamFoundation.pro to point to it), or use the binaries from the releases section.
* Put the binaries in a folder named TeamFoundation in Qt Creators plugins directory (default is C:\Qt\Tools\QtCreator\lib\qtcreator\plugins). For Qt Creator 3.2 and newer, all plugins are stored directly in the plugin directory. For Qt 3.5 and later, the initial checkout wizard (icon.png and wizard.json) must be moved to C:\Qt\Tools\QtCreator\share\qtcreator\templates\wizards\projects\vcs\teamfoundation (or where you have installed Qt Creator).
