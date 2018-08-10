import React from 'react';
import PropTypes from 'prop-types'

const ContentList = ({content}) => {  
  return (
    <ul className='list-group'>
      {content.map(item => {
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

ContentList.propTypes = {  
  content: PropTypes.array.isRequired
};

export default ContentList; 
