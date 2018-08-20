import React from 'react';
import PropTypes from 'prop-types'
import { Link } from 'react-router-dom'

const LanguagesList = ({ languages, onReload }) => {
  return (
    <React.Fragment>
      {languages.error ? (
        <React.Fragment>
          Error : {languages.error}
        </React.Fragment>
      ) : (
        <ul className='list-group'>
          {languages.items.map(language => {
            const id = language['_id']['$oid'];
            return (
              <li className='list-group-item' key={id}>
                <span>{language.name}</span>
                <Link to={`/languages/${id}/edit`}>Edit</Link>
                <Link to={`/languages/${id}/delete`}>Delete</Link>
              </li>
            );
          })}
        </ul>
      )}
      <button onClick={onReload}>
        Load
      </button>
    </React.Fragment>
  );
};

LanguagesList.propTypes = {
  languages: PropTypes.object.isRequired,
  onReload: PropTypes.func.isRequired
};

export default LanguagesList;
