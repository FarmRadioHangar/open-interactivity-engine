import React from 'react';
import PropTypes from 'prop-types'
import { Link } from 'react-router-dom'

const LanguagesList = ({languages}) => {  
  return (
    <ul className='list-group'>
      {languages.map(language => {
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
  );
};

LanguagesList.propTypes = {  
  languages: PropTypes.array.isRequired
};

export default LanguagesList; 
