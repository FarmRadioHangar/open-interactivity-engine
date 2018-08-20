export function param(json) {
  return '?' + Object.keys(json).map(function(key) {
    return encodeURIComponent(key) + '=' + encodeURIComponent(json[key]);
  }).join('&');
}

function camelize(str) {
  return str.replace(/(_\w)/g, (c) => `${c[1].toUpperCase()}`);
}

function decamelize(str) {
  return str.replace(/([A-Z])/g, (c) => `_${c.toLowerCase()}`);
}

export function underscoreKeys(data) {
  let result = {};
  Object.keys(data).map(key => {
    result[decamelize(key)] = data[key];
  });
  return result;
}
