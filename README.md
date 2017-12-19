Team Foundation Server plugin for Qt Creator
============================================

[![Build status](https://ci.appveyor.com/api/projects/status/legfgybaww8qlinq/branch/master?svg=true
)](https://ci.appveyor.com/project/jesperhh/teamfoundation)


Requirements
------------
Either
* Team Explorer (tested with 2012 edition, should work with later releases)
Or
* Visual Studio (tested with 2012 and 2013 edition, should work with later releases)

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
* Either compile from source, or use the binaries from the releases section.
  - To build from scratch, you can follow the steps in appveyor.yml
* Put the dll in the Qt Creators plugins directory
  - (install directory)\QtCreator\lib\qtcreator\plugins
* Put the wizard (icon.png and wizard.json) in a folder called teamfoundation under the vcs wizards:
  - (install directory)\QtCreator\share\qtcreator\templates\wizards\projects\vcs\teamfoundation
