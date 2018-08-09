import React from 'react';  
import PropTypes from 'prop-types'
import { connect } from 'react-redux';  
import * as languagesActions from '../actions/languagesActions';
import LanguagesList from './languagesList';

class LanguagesIndex extends React.Component {  
  componentDidMount() {
    this.props.dispatch(languagesActions.loadLanguages());
  }
  render() {
    return(
      <LanguagesList languages={this.props.languages} />
    );
  }
}

LanguagesIndex.propTypes = {
  languages: PropTypes.array.isRequired
};

function mapStateToProps(state, ownProps) {
  return {
    languages: state.languages
  };
} 

export default connect(mapStateToProps)(LanguagesIndex);  
