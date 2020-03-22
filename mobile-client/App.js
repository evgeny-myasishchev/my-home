import {Button, Header, Icon} from 'react-native-elements';
import React from 'react';

function App() {
  return (
    <>
      <Header
        leftComponent={{icon: 'menu', color: '#fff'}}
        centerComponent={{text: 'MY TITLE', style: {color: '#fff'}}}
        rightComponent={{icon: 'home', color: '#fff'}}
      />
      <Button title="Hey!" />
      <Icon name="rowing" />
    </>
  );
}

export default App;
