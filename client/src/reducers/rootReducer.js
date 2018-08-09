import { combineReducers } from 'redux';
import campaigns from './campaignsReducer';
import content from './contentReducer';
import audience from './audienceReducer';
import languages from './languagesReducer';

const rootReducer = combineReducers({
  campaigns,
  content,
  audience,
  languages
});

export default rootReducer;
