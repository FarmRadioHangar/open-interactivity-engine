import './utils';

class Api {

  get(resource, params) {
    const url = `${process.env.API_URL}/${resource}${params ? utils.param(params) : ''}`;
    console.log(`GET ${url}`);
    return fetch(`${url}`)
      .then(response => {
        return response.json();
      })
      .catch(error => {
        console.error(error);
        return error;
      });
  }

  post(resource, data) {
    console.log(`POST ${process.env.API_URL}/${resource}`);
    return fetch(`${process.env.API_URL}/${resource}`, {
      method: 'POST',
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(data)
    }).then(response => {
        return response.json();
      })
      .catch(error => {
        console.error(error);
        return error;
      });
  }

  delete(resource) {
    console.log(`DELETE ${process.env.API_URL}/${resource}`);
    return fetch(`${process.env.API_URL}/${resource}`, {
      method: 'DELETE'
    })
      .catch(error => {
        console.error(error);
        return error;
      });
  }

}

export default new Api();
