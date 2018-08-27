const FETCH_ERROR_MSG = 'Error talking to API. Is the server running and accepting connections?';

export default class Api {

  get(resource, params) {
    const query = params ? `?${this.toQueryString(params)}` : '';
    const url = `${process.env.API_URL}/${resource}${query}`;
    console.log(`GET ${url}`);
    return fetch(url)
      .catch(err => { throw new Error(FETCH_ERROR_MSG); })
      .then(this.sendResponse);
  }

  post(resource, data) {
    const url = `${process.env.API_URL}/${resource}`;
    console.log(`POST ${url}`);
    const options = {
      method: 'POST',
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(data)
    };
    return fetch(url, options)
      .catch(err => { throw new Error(FETCH_ERROR_MSG); })
      .then(this.sendResponse);
  }

  put(resource, data) {
    const url = `${process.env.API_URL}/${resource}`;
    console.log(`PUT ${url}`);
    const options = {
      method: 'PUT',
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(data)
    };
    return fetch(url, options)
      .catch(err => { throw new Error(FETCH_ERROR_MSG); })
      .then(this.sendResponse);
  }

  httpDelete(resource) {
    const url = `${process.env.API_URL}/${resource}`;
    console.log(`DELETE ${url}`);
    return fetch(url, { method: 'DELETE' })
      .catch(err => { throw new Error(FETCH_ERROR_MSG); })
      .then(this.sendResponse);
  }

  toQueryString(obj) {
    return Object
      .keys(obj)
      .map(key => encodeURIComponent(key) + '=' + encodeURIComponent(obj[key]))
      .join('&');
  }

  sendResponse(response) {
    return response.json().then(json => {
      const { status, ok } = response;
      return { ...json, status, ok };
    });
  }

  static toSnakeCase(payload) {
    return Api.convertCase(payload, (str) =>
      str.replace(/([A-Z])/g, (capture) => `_${capture.toLowerCase()}`)
    );
  }

  static toCamelCase(payload) {
    return Api.convertCase(payload, (str) => '_id' === str ? str :
      str.replace(/(_\w)/g, (capture) => `${capture[1].toUpperCase()}`)
    );
  }

  static convertCase(data, fun) {
    if ('object' === typeof(data)) {
      if (Array.isArray(data)) {
        return data.map(element => Api.convertCase(element, fun));
      } else {
        let result = {};
        Object.keys(data).forEach(key => {
          result[fun(key)] = Api.convertCase(data[key], fun);
        });
        return result;
      }
    }
    return data;
  }

}
