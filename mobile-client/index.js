// this one has to be top
import 'react-native-gesture-handler';

import {AppRegistry} from 'react-native';
import {NavigationContainer} from '@react-navigation/native';
import {ThemeProvider} from 'react-native-elements';
import React from 'react';

import {name as appName} from './app.json';
import App from './App';

function Main() {
  return (
    <NavigationContainer>
      <ThemeProvider>
        <App />
      </ThemeProvider>
    </NavigationContainer>
  );
}

AppRegistry.registerComponent(appName, () => Main);
