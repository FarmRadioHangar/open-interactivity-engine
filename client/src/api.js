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
}

export default new Api();
