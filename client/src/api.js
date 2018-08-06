class Api {
  static getCampaigns() {
    return fetch('http://localhost:9080/campaigns')
      .then(response => {
        return response.json();
      })
      .catch(error => {
        console.error(error);
        return error;
      });
  }
}

export default Api;
