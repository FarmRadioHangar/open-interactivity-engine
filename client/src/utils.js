export function param(json) {
  return '?' + Object.keys(json).map(function(key) {
    return encodeURIComponent(key) + '=' + encodeURIComponent(json[key]);
  }).join('&');
}

function camelize(str) {
  if ('_id' === str)
    return str;
  return str.replace(/(_\w)/g, (c) => `${c[1].toUpperCase()}`);
}

function decamelize(str) {
  return str.replace(/([A-Z])/g, (c) => `_${c.toLowerCase()}`);
}

export function translKeys(data, camelCase = false) {
  let result = {},
      fun = camelCase ? camelize : decamelize;
  Object.keys(data).map(key => {
    result[fun(key)] = data[key];
  });
  return result;
}
