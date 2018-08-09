import React from 'react';
import PropTypes from 'prop-types'

const LanguagesList = ({languages}) => {  
  return (
    <ul className='list-group'>
      {languages.map(language =>
        <li className='list-group-item' key={language['_id']['$oid']}>
          {language.name}
        </li>
      )}
    </ul>
  );
};

LanguagesList.propTypes = {  
  languages: PropTypes.array.isRequired
};

export default LanguagesList; 
