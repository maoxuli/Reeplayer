# Reeplayer 

## Build on Mac OSX

1. Install Xcode, for clang compiler; 

2. Install QT, for QT SDK and QtCreator; 

3. Install GStreamer runtime and devel packages;

4. Install jsoncpp and libjson-rpc-cpp with homebrew; 

5. Build in QtCreator or with qmake & make 

Check to make sure "/usr/local/bin" is in the PATH, which is necessary for pkg-config.

otool -L Reeplayer.app/Contents/MacOS/Reeplayer

6. Package 

sudo macdeployqt Reeplayer.app/ -dmg



