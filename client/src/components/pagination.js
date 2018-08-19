import React from 'react';
import List from '../list';

const Pagination = ({ pageSize, total, offset, onChange }) => {
  const pageCount = Math.ceil(total/pageSize);
  const currentPage = offset/pageSize + 1;
  console.log('offset = ' + offset);
  console.log('pageSize = ' + pageSize);
  console.log('current = ' + currentPage);
  return (
    <ul>
      {List.enumFrom(1).take(pageCount).toArray().map(page =>
        <li key={page}>
          {page == currentPage ? (
            <span>{page}</span>
          ) : (
            <a href='#' onClick={() => onChange(page)}>
              {page}
            </a>
          )}
        </li>
      )}
    </ul>
  );
};

export default Pagination;
