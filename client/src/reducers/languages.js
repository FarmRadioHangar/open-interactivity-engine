import initialState from './initialState';

export default function(state = initialState.languages, action) {
  switch(action.type) {
    case 'FETCH_LANGUAGES_ITEM_REQUEST': {
      const { item } = initialState.languages;
      return { ...state, item, itemFetching: true };
    }
    case 'FETCH_LANGUAGES_ITEM_SUCCESS': {
      const { item } = action;
      return { 
        ...state, item,
        itemFetching: false,
        itemError: null
      };
    }
    case 'FETCH_LANGUAGES_ITEM_FAILURE': {
      const { item } = initialState.languages;
      const { error } = action;
      return { 
        ...state, item,
        itemFetching: false,
        itemError: error
      };
    }
    case 'FETCH_LANGUAGES_PAGE_REQUEST': {
      const { page, total } = initialState.languages;
      const { offset } = action;
      return { ...state, page, total, offset, pageFetching: true };
    }
    case 'FETCH_LANGUAGES_PAGE_SUCCESS': {
      const { page, total } = action;
      return {
        ...state, page, total,
        pageFetching: false,
        pageError: null
      };
    }
    case 'FETCH_LANGUAGES_PAGE_FAILURE': {
      const { items, total } = initialState.languages;
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
