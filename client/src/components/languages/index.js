import React from 'react';
import PropTypes from 'prop-types';
import { Link } from 'react-router-dom';
import { connect } from 'react-redux';
import history from '../../history';
import Pagination from '../pagination';

const LanguagesList = ({ languages }) => {
  return (
    <React.Fragment>
      <ul>
        {languages.map(language => {
          const id = language._id['$oid'];
          return (
            <li key={id}>
              <span>{language.name} ({language.tag})</span>
              <Link to={`/languages/${id}/view`}>View</Link>
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
  languages: PropTypes.array.isRequired
};

const LanguagesIndex = ({ pagination, page, pageFetching, pageError }) => {
  return (
    <React.Fragment>
      {pageFetching ? (
        <span>Please wait...</span>
      ) : (
        <React.Fragment>
          {pageError ? (
            <React.Fragment>
              {pageError.message}
            </React.Fragment>
          ) : (
            <React.Fragment>
              <LanguagesList languages={page} />
              {pagination && (
                <Pagination {...pagination} onChange={(page) => { 
                  history.push(`/languages/page/${page}`); 
                }} />
              )}
              <button onClick={() => {
                history.push('/languages/create');
              }}>
                Create language
              </button>
            </React.Fragment>
          )}
        </React.Fragment>
      )}
    </React.Fragment>
  );
};

LanguagesIndex.propTypes = {
  pagination: PropTypes.object,
  page: PropTypes.array,
  pageFetching: PropTypes.bool,
  pageError: PropTypes.object
};

function mapStateToProps(state, ownProps) {
  return state.languages;
}

export default connect(mapStateToProps)(LanguagesIndex);
