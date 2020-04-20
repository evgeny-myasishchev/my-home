import {Button, Header, Icon} from 'react-native-elements';
import {View} from 'react-native';
import {createDrawerNavigator} from '@react-navigation/drawer';
import React from 'react';

function HomeScreen({navigation}) {
  return (
    <View style={{flex: 1, alignItems: 'center', justifyContent: 'center'}}>
      <Button
        onPress={() => navigation.navigate('Notifications')}
        title="Go to notifications"
      />
    </View>
  );
}

function NotificationsScreen({navigation}) {
  return (
    <View style={{flex: 1, alignItems: 'center', justifyContent: 'center'}}>
      <Button onPress={() => navigation.goBack()} title="Go back home" />
    </View>
  );
}

function createScreenComponent({component: Component, title}) {
  return ({...props}) => {
    const {navigation} = props;
    return (
      <>
        <Header
          leftComponent={
            <Icon
              name="menu"
              size={30}
              type="entypo"
              iconStyle={{color: '#fff', paddingLeft: 10}}
              onPress={navigation.toggleDrawer}
            />
          }
          centerComponent={{text: title, style: {color: '#fff'}}}
          rightComponent={{icon: 'home', color: '#fff'}}
        />
        <Component {...props} />
      </>
    );
  };
}

const Drawer = createDrawerNavigator();
const HomeScreenComponent = createScreenComponent({
  title: 'My Home',
  component: HomeScreen,
});
const NotificationsScreenComponent = createScreenComponent({
  title: 'My Notifications',
  component: NotificationsScreen,
});

export default function DrawerNavigator() {
  return (
    <>
      <Drawer.Navigator initialRouteName="Home">
        <Drawer.Screen name="Home" component={HomeScreenComponent} />
        <Drawer.Screen
          name="Notifications"
          component={NotificationsScreenComponent}
          options={{
            drawerIcon: ({tintColor}) => (
              <Icon
                name="cog"
                size={30}
                iconStyle={{
                  width: 30,
                  height: 30,
                }}
                type="entypo"
                color={tintColor}
              />
            ),
          }}
        />
      </Drawer.Navigator>
    </>
  );
}
