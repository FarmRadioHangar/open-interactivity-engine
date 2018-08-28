import initialState from './initialState';

export default function(state = initialState.content, action) {
  switch(action.type) {
    case 'FETCH_CONTENT_PAGE_REQUEST': {
      const { offset } = action;
      return { ...state, offset, pageFetching: true };
    }
    case 'FETCH_CONTENT_PAGE_SUCCESS': {
      const { page , total } = action;
      return {
        ...state, page, total,
        pageFetching: false,
        pageError: null
      };
    }
    case 'FETCH_CONTENT_PAGE_FAILURE': {
      const { items, total } = initialState.content;
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
