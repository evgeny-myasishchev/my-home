import {NavigationContainer} from '@react-navigation/native';
import * as React from 'react';

import DrawerNavigator from './src/navigation/DrawerNavigator';

export default function App() {
  return (
    <NavigationContainer>
      <DrawerNavigator />
    </NavigationContainer>
  );
}
