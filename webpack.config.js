const webpack = require('webpack')
const path = require('path')
const ExtractTextWebpackPlugin = require('extract-text-webpack-plugin')
const UglifyJsPlugin = require('uglifyjs-webpack-plugin')
const OptimizeCSSAssets = require('optimize-css-assets-webpack-plugin')

let config = {
  entry: './html/src/index.js',
  output: {
    filename: 'bundle.js',
    path: path.resolve(__dirname, 'html/public/'),
  },
  resolve: {
    extensions: ['.js', '.jsx', '.json', '.scss', '.css', '.jpeg', '.jpg', '.gif', '.png'],
    alias: {
      images: path.resolve(__dirname, 'html/src/assets/images')
    }
  },
  module: {
    rules: [{
      test: /\.js$/,
      exclude: /node_modules/,
      loader: "babel-loader"
      },
      {
        test: /\.scss$/,
        use: ExtractTextWebpackPlugin.extract({
          use: ['css-loader', 'sass-loader'],
          fallback: 'style-loader'
        })
      },
      {
        test: /\.jsx$/,
        loader: 'babel-loader',
        exclude: /node_modules/
      },
      {
        test: /\.(jpe?g|png|gif|svg)$/i,
        loaders: ['file-loader?context=html/src/assets/images/&name=images/[path][name].[ext]', {
          loader: 'image-webpack-loader',
          query: {
            mozjpeg: {
              progressive: true
            },
            gifsicle: {
              interlaced: false
            },
            optipng: {
                optimizationLevel: 4
            },
            pngquant: {
              quality: '75-90',
              speed: 3
            }
          }
        }]
      }
    ]
  },
  plugins: [
    new ExtractTextWebpackPlugin('style.css')
  ]
}

module.exports = config

if(process.env.NODE_ENV === 'production') {
  module.exports.plugins.push(
      new webpack.optimize.UglifyJsPlugin(),
      new OptimizeCSSAssets()
  )
}

