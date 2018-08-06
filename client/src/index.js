import React from 'react';
import ReactDOM from 'react-dom';
import { BrowserRouter as Router, Route, Link } from 'react-router-dom';
import { Provider } from 'react-redux';
import configureStore from './store/configureStore';
import { loadCampaigns } from './actions/campaignsActions';
import CampaignsIndex from './components/campaignsIndex';

const store = configureStore();

store.dispatch(loadCampaigns());

const BasicExample = () => (
  <Router>
    <div>
      <ul>
        <li>
          <Link to='/'>Home</Link>
        </li>
        <li>
          <Link to='/campaigns'>Campaigns</Link>
        </li>
        <li>
          <Link to='/content'>Content</Link>
        </li>
        <li>
          <Link to='/audience'>Audience</Link>
        </li>
        <li>
          <Link to='/languages'>Languages</Link>
        </li>
        <li>
          <Link to='/settings'>Settings</Link>
        </li>
      </ul>
      <hr />
      <Route exact path='/' component={Home} />
      <Route path='/campaigns' component={Campaigns} />
      <Route path='/content' component={Content} />
      <Route path='/audience' component={Audience} />
      <Route path='/languages' component={Languages} />
      <Route path='/settings' component={Settings} />
    </div>
  </Router>
);

const Home = () => (
  <div>
    <h2>Home</h2>
  </div>
);

const Campaigns = () => (
  <div>
    <h2>Campaigns</h2>
    <CampaignsIndex />
  </div>
);

const Content = () => (
  <div>
    <h2>Content</h2>
  </div>
);

const Audience = () => (
  <div>
    <h2>Audience</h2>
  </div>
);

const Languages = () => (
  <div>
    <h2>Languages</h2>
  </div>
);

const Settings = () => (
  <div>
    <h2>Settings</h2>
  </div>
);

ReactDOM.render(
  <Provider store={store}>
    <BasicExample />
  </Provider>, 
  document.getElementById('app')
);
