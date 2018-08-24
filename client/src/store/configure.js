import { createStore, applyMiddleware } from 'redux';
import rootReducer from '../reducers/root';
import sagaMiddleware from '../sagas/middleware';

export default function() {
  return createStore(
    rootReducer,
    applyMiddleware(sagaMiddleware)
  );
}
