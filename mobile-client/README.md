# My Home Mobile Client

React Native mobile client (Android)

## Dev

Make sure to have [nvm](https://github.com/nvm-sh/nvm) installed.

Follow [React Native CLI Quickstart](https://reactnative.dev/docs/getting-started).

```
#Activate node
nvm use

#Install deps
npm i

#Start dev webserver (prefer a separate shell)
npm run start
```

Prepare emulator with Android Studio. Write it's name to `.avd-name` file (one time only)

Then emulator and then android app:
```
npm run android-emu&
npm run android
```