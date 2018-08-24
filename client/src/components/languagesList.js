import React from 'react';
import PropTypes from 'prop-types'
import { Link } from 'react-router-dom'

const LanguagesList = ({ languages }) => {
  return (
    <React.Fragment>
      <ul className='list-group'>
        {languages.items.map(language => {
          const id = language._id['$oid'];
          return (
            <li className='list-group-item' key={id}>
              <span>{language.name} ({language.isoCode})</span>
              <Link to={`/languages/${id}/edit`}>Edit</Link>
              <Link to={`/languages/${id}/delete`}>Delete</Link>
            </li>
          );
        })}
      </ul>
    </React.Fragment>
  );
};

LanguagesList.propTypes = {
  languages: PropTypes.object.isRequired
};

export default LanguagesList;
