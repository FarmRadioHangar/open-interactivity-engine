import * as utils from './utils';

class Api {

  get(resource, params) {
    const url = `${process.env.API_URL}/${resource}${params ? utils.param(params) : ''}`;
    console.log(`GET ${url}`);
    let response;
    return fetch(url)
      .then(res => {
        response = res;
        console.log(response);
        return response.json();
      })
      .then(json => {
        const { status, ok } = response;
        return { ...json, status, ok };
      });
  }

  post(resource, data) {
    const url = `${process.env.API_URL}/${resource}`;
    console.log(`POST ${url}`);
    let response;
    return fetch(url, {
        method: 'POST',
        headers: {
          'Accept': 'application/json',
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
      })
      .then(res => {
        response = res;
        console.log(response);
        return response.json();
      })
      .then(json => {
        const { status, ok } = response;
        return { ...json, status, ok };
      });
  }

  put(resource, data) {
    const url = `${process.env.API_URL}/${resource}`;
    console.log(`PUT ${url}`);
    let response;
    return fetch(url, {
        method: 'PUT',
        headers: {
          'Accept': 'application/json',
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
      })
      .then(res => {
        response = res;
        console.log(response);
        return response.json();
      })
      .then(json => {
        const { status, ok } = response;
        return { ...json, status, ok };
      });
  }

  delete(resource) {
    const url = `${process.env.API_URL}/${resource}`;
    console.log(`DELETE ${url}`);
    let response;
    return fetch(url, { method: 'DELETE' })
      .then(res => {
        response = res;
        console.log(response);
        return response.json();
      })
      .then(json => {
        const { status, ok } = response;
        return { ...json, status, ok };
      });
  }

}

export default new Api();
