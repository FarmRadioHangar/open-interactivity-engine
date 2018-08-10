import React from 'react';
import PropTypes from 'prop-types'

const AudienceList = ({audience}) => {  
  return (
    <ul className='list-group'>
      {audience.map(item => {
        const id = item['_id']['$oid'];
        return (
          <li className='list-group-item' key={id}>
            {item.name}
          </li>
        );
      })}
    </ul>
  );
};

AudienceList.propTypes = {  
  audience: PropTypes.array.isRequired
};

export default AudienceList; 
