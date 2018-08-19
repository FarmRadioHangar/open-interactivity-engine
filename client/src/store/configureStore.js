import { createStore, applyMiddleware } from 'redux';  
import rootReducer from '../reducers/rootReducer';  
import sagaMiddleware from '../sagas/sagaMiddleware';

export default function() {  
  return createStore(
    rootReducer,
    applyMiddleware(sagaMiddleware)
  );
}
