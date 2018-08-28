import initialState from './initialState';

export default function(state = initialState.campaigns, action) {
  switch(action.type) {
    case 'FETCH_CAMPAIGNS_PAGE_REQUEST': {
      const { offset } = action;
      return { ...state, offset, pageFetching: true };
    }
    case 'FETCH_CAMPAIGNS_PAGE_SUCCESS': {
      const { page , total } = action;
      return {
        ...state, page, total,
        pageFetching: false,
        pageError: null
      };
    }
    case 'FETCH_CAMPAIGNS_PAGE_FAILURE': {
      const { items, total } = initialState.campaigns;
      const { error } = action;
      return {
        ...state, items, total,
        pageFetching: false,
        pageError: error
      };
    }
    default:
      return state;
  }
}
