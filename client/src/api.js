class Api {
  getCampaigns() {
    return fetch(`${process.env.API_URL}/campaigns`)
      .then(response => {
        return response.json();
      })
      .catch(error => {
        console.error(error);
        return error;
      });
  }

  getContent() {
    return fetch(`${process.env.API_URL}/content`)
      .then(response => {
        return response.json();
      })
      .catch(error => {
        console.error(error);
        return error;
      });
  }

  getAudience() {
    return fetch(`${process.env.API_URL}/audience`)
      .then(response => {
        return response.json();
      })
      .catch(error => {
        console.error(error);
        return error;
      });
  }

  getLanguages() {
    return fetch(`${process.env.API_URL}/languages`)
      .then(response => {
        console.log(response);
        return response.json();
      })
      .catch(error => {
        console.error(error);
        return error;
      });
  }

  postLanguage(data) {
    return fetch(`${process.env.API_URL}/languages`, {
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
