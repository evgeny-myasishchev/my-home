import React from 'react';
import {Icon, Button, ThemeProvider} from 'react-native-elements';

function App() {
  return (
    <ThemeProvider>
      <Button title="Hey!" />
      <Icon name="rowing" />
    </ThemeProvider>
  );
}

export default App;
