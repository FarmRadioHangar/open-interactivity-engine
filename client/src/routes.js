import React from 'react';
import { Route, Switch } from 'react-router';

const X = () => (
  <div>
    X
  </div>
);

const Home = () => (
  <div>
    Home
  </div>
);

const NotFound = () => (
  <div>
    Not found
  </div>
);

const Routes = () => (
  <Switch>
    <Route exact path='/' component={Home} />
    <Route exact path='/campaigns' component={X} />
    <Route exact path='/campaigns/page/:page' component={X} />
    <Route exact path='/campaigns/create' component={X} />
    <Route exact path='/campaigns/:id/view' component={X} />
    <Route exact path='/campaigns/:id/edit' component={X} />
    <Route exact path='/campaigns/:id/delete' component={X} />
    <Route exact path='/content' component={X} />
    <Route exact path='/content/page/:page' component={X} />
    <Route exact path='/content/create' component={X} />
    <Route exact path='/content/:id/view' component={X} />
    <Route exact path='/content/:id/edit' component={X} />
    <Route exact path='/content/:id/delete' component={X} />
    <Route exact path='/audience' component={X} />
    <Route exact path='/audience/page/:page' component={X} />
    <Route exact path='/audience/create' component={X} />
    <Route exact path='/audience/:id/view' component={X} />
    <Route exact path='/audience/:id/edit' component={X} />
    <Route exact path='/audience/:id/delete' component={X} />
    <Route exact path='/languages' component={X} />
    <Route exact path='/languages/page/:page' component={X} />
    <Route exact path='/languages/create' component={X} />
    <Route exact path='/languages/:id/view' component={X} />
    <Route exact path='/languages/:id/edit' component={X} />
    <Route exact path='/languages/:id/delete' component={X} />
    <Route exact path='/settings' component={X} />
    <Route component={NotFound} />
  </Switch>
);

export default Routes;
