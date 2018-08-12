class Api {

  get(resource) {
    console.log(`GET ${process.env.API_URL}/${resource}`);
    return fetch(`${process.env.API_URL}/${resource}`)
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

}

export default new Api();
