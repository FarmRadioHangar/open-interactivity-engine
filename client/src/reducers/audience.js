import initialState from './initialState';

export default function(state = initialState.audience, action) {
  switch(action.type) {
    case 'FETCH_AUDIENCE_PAGE_REQUEST': {
      const { offset } = action;
      return { ...state, offset, pageFetching: true };
    }
    case 'FETCH_AUDIENCE_PAGE_SUCCESS': {
      const { page , total } = action;
      return {
        ...state, page, total,
        pageFetching: false,
        pageError: null
      };
    }
    case 'FETCH_AUDIENCE_PAGE_FAILURE': {
      const { items, total } = initialState.audience;
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
