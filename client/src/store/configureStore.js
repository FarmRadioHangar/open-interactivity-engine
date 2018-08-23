import { createStore, applyMiddleware, compose } from 'redux';
import { connectRouter, routerMiddleware } from 'connected-react-router';
import rootReducer from '../reducers/root';
import sagaMiddleware from '../sagas/sagaMiddleware';
import history from '../history';

export default function() {
  return createStore(
    connectRouter(history)(rootReducer),
    compose(applyMiddleware(
      routerMiddleware(history),
      sagaMiddleware
    ))
  );
}
