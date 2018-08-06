import { combineReducers } from 'redux';
import campaigns from './campaignsReducer';

const rootReducer = combineReducers({
  campaigns
});

export default rootReducer;
