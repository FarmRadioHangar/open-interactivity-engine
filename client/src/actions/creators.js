import { createFormAction } from 'redux-form-saga';

export function fetchPageRequest(resource, offset = 0) {
  return { type: `FETCH_${resource.toUpperCase()}_PAGE_REQUEST`, offset };
}

export function fetchPageSuccess(resource, page, total) {
  return { type: `FETCH_${resource.toUpperCase()}_PAGE_SUCCESS`, page, total };
}

export function fetchPageFailure(resource, error) {
  return { type: `FETCH_${resource.toUpperCase()}_PAGE_FAILURE`, error };
}

export function fetchItemRequest(resource) {
  return { type: `FETCH_${resource.toUpperCase()}_ITEM_REQUEST` };
}

export function fetchItemSuccess(resource, item) {
  return { type: `FETCH_${resource.toUpperCase()}_ITEM_SUCCESS`, item };
}

export function fetchItemFailure(resource, error) {
  return { type: `FETCH_${resource.toUpperCase()}_ITEM_FAILURE`, error };
}

export const createLanguageAction = createFormAction('CREATE_LANGUAGE');
export const updateLanguageAction = createFormAction('UPDATE_LANGUAGE');
export const deleteLanguageAction = createFormAction('DELETE_LANGUAGE');
