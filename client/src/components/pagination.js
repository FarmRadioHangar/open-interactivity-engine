import React from 'react';
import PropTypes from 'prop-types';
import List from '../utils/list';

const Pagination = ({ pageCount, currentPage, onChange }) => {
  if (pageCount < 2) {
    return <React.Fragment />
  }
  return (
    <ul>
      {List.enumFrom(1).take(pageCount).toArray().map(page =>
        <li key={page}>
          {page == currentPage ? (
            <span>{page}</span>
          ) : (
            <a href='#' onClick={e => { e.preventDefault(); onChange(page) }}>
              {page}
            </a>
          )}
        </li>
      )}
    </ul>
  );
};

Pagination.propTypes = {
  pageCount: PropTypes.number.isRequired,
  currentPage: PropTypes.number.isRequired,
  onChange: PropTypes.func
};

Pagination.defaultProps = {
  onChange: () => {}
};

export default Pagination;
