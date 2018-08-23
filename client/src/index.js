import React from 'react';
import ReactDOM from 'react-dom';
import { Provider } from 'react-redux';
import { Route, Switch } from 'react-router';
import { Link } from 'react-router-dom';
import { ConnectedRouter } from 'connected-react-router';
import { rootSaga } from './sagas/sagas';
import sagaMiddleware from './sagas/sagaMiddleware';
import configureStore from './store/configureStore';
import history from './history';
import * as languagesActions from './actions/languagesActions';

const store = configureStore();

sagaMiddleware.run(rootSaga);

const Main = () => (
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
    <Switch>
      <Route exact path='/' render={() => (<div>Home</div>)} />
      <Route exact path='/languages' component={Languages} />
      <Route render={() => (<div>Nothing</div>)} />
    </Switch>
  </div>
);

const Languages = ({ match }) => {
  console.log(JSON.stringify(match));
  return (
    <div>
      <div>
        Languages
      </div>
      <button onClick={() => {
        store.dispatch(languagesActions.fetchLanguages(0, 20));
      }}>
        Fetch languages
      </button>
    </div>
  );
}

ReactDOM.render(
  <Provider store={store}>
    <ConnectedRouter history={history}>
      <Main />
    </ConnectedRouter>
  </Provider>,
  document.getElementById('app')
);
