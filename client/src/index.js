import 'babel-polyfill';
import React from 'react';
import ReactDOM from 'react-dom';
import { Link, Router } from 'react-router-dom';
import { rootSaga } from './sagas/sagas';
import { Provider } from 'react-redux';
import configureStore from './store/configure';
import sagaMiddleware from './sagas/middleware';
import history from './history';
import Routes from './routes';

const store = configureStore();

sagaMiddleware.run(rootSaga);

const Main = () => (
  <React.Fragment>
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
  </React.Fragment>
);

ReactDOM.render(
  <Provider store={store}>
    <Router history={history}>
      <React.Fragment>
        <Main />
        <Routes />
      </React.Fragment>
    </Router>
  </Provider>,
  document.getElementById('app')
);
