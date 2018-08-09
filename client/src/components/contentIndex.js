import React from 'react';  
import PropTypes from 'prop-types'
import { connect } from 'react-redux';  
import * as contentActions from '../actions/contentActions';
import ContentList from './contentList';

class ContentIndex extends React.Component {  
  componentDidMount() {
    this.props.dispatch(contentActions.loadContent());
  }
  render() {
    return(
      <ContentList content={this.props.content} />
    );
  }
}

ContentIndex.propTypes = {
  content: PropTypes.array.isRequired
};

function mapStateToProps(state, ownProps) {
  return {
    content: state.content
  };
} 

export default connect(mapStateToProps)(ContentIndex);  
