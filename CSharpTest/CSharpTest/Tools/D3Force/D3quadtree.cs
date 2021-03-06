﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CSharpTest.Model;

namespace CSharpTest.Tools
{
    class D3quadtree
    {
        /// <summary>
        /// 根结点
        /// </summary>
        public QuadtreeNode _root;
        /// <summary>
        /// x坐标左值
        /// </summary>
        public double _x0;
        /// <summary>
        /// y坐标下值
        /// </summary>
        public double _y0;
        /// <summary>
        /// x坐标右值
        /// </summary>
        public double _x1;
        /// <summary>
        /// y坐标上值
        /// </summary>
        public double _y1;

        /// <summary>
        /// 初始化四叉树
        /// </summary>
        /// <param name="nodes">节点</param>
        /// <returns></returns>
        public D3quadtree(List<ForceNode> nodes)
        {
            Quadtree(0.0, 0.0, 0.0, 0.0);
            if (nodes.Count > 0)
            {
                addAll(nodes);
            }
        }

        void Quadtree(double x0, double y0, double x1, double y1)
        {
            this._x0 = x0;
            this._y0 = y0;
            this._x1 = x1;
            this._y1 = y1;
            //this._root = null;
        }

        /// <summary>
        /// 添加所有节点
        /// </summary>
        /// <param name="data">节点列表</param>
        void addAll(List<ForceNode> data)
        {
            int n = data.Count;
            double x = 0.0, y = 0.0;        //当前节点的x,y坐标
            double x0 = double.MaxValue, y0 = double.MaxValue;      //最小的x,y坐标
            double x1 = double.MinValue, y1 = double.MinValue;      //最大的x,y坐标

            var xz = new List<double>();
            var yz = new List<double>();
            for (int i = 0; i < n; i++)
            {
                xz.Add(0.0);
                yz.Add(0.0);
            }
            //初始化及坐标及极值
            for (int i = 0; i < n; i++)
            {
                var d = data[i];
                x = d.x;
                y = d.y;
                xz[i] = x;
                yz[i] = y;
                if (x < x0)
                    x0 = x;
                if (x > x1)
                    x1 = x;
                if (y < y0)
                    y0 = y;
                if (y > y1)
                    y1 = y;
            }

            //如果极值不存在，则使用已存在的极值
            if (x1 < x0)
            {
                x0 = this._x0;
                x1 = this._x1;
            }
            if (y1 < y0)
            {
                y0 = this._y0;
                y1 = this._y1;
            }

            //扩展树以覆盖新节点
            cover(x0, y0);
            cover(x1, y1);

            //添加新节点
            for (int i = 0; i < n; i++)
            {
                add(xz[i], yz[i], data[i]);
            }
        }

        /// <summary>
        /// 扩展树以覆盖新节点
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        void cover(double x, double y)
        {
            //忽略无效点
            if (x == 0.0 || y == 0.0)
                return;

            double x0 = this._x0;
            double x1 = this._x1;
            double y0 = this._y0;
            double y1 = this._y1;

            if (x0 == x1 && y0 == y1)
            {
                x0 = Math.Floor(x);
                x1 = x0 + 1;
                y0 = Math.Floor(y);
                y1 = y0 + 1;
            }
            //判断点是否不在现有四叉树范围内
            else if (x0 > x || x > x1 || y0 > y || y > y1)
            {
                //不再其范围内则扩展4四叉树至包含该点
                var z = x1 - x0;            //叶子节点间隔
                if (this._root == null)
                    this._root = new QuadtreeNode();
                var node = this._root;

                //判断节点类型
                //(y < (y0 + y1) / 2) << 1 | (x < (x0 + x1) / 2)
                bool isY = y < (y0 + y1) / 2;
                bool isX = x < (x0 + x1) / 2;
                int i = 0;
                if (isY)
                    i = 1 << 1;
                else
                    i = 0 << 1;
                if (isX)
                    i = i | 1;
                else
                    i = i | 0;

                //初始化节点分布
                switch (i)
                {
                    case 0:
                        do
                        {
                            QuadtreeNode parent = new QuadtreeNode();
                            parent.children = new QuadtreeNode[4];
                            parent.children[i] = node;
                            node = parent;
                            z *= 2;
                            x1 = x0 + z;
                            y1 = y0 + z;
                            
                        } while (x>x1||y>y1);
                        break;
                    case 1:
                        do
                        {
                            QuadtreeNode parent = new QuadtreeNode();
                            parent.children = new QuadtreeNode[4];
                            parent.children[i] = node;
                            node = parent;
                            z *= 2;
                            x1 = x0 - z;
                            y1 = y0 + z;
                        } while (x > x1 || y > y1);
                        break;
                    case 2:
                        do
                        {
                            QuadtreeNode parent = new QuadtreeNode();
                            parent.children = new QuadtreeNode[4];
                            parent.children[i] = node;
                            node = parent;
                            z *= 2;
                            x1 = x0 + z;
                            y1 = y0 - z;
                        } while (x > x1 || y > y1);
                        break;
                    case 3:
                        do
                        {
                            QuadtreeNode parent = new QuadtreeNode();
                            parent.children = new QuadtreeNode[4];
                            parent.children[i] = node;
                            node = parent;
                            z *= 2;
                            x1 = x0 - z;
                            y1 = y0 - z;
                        } while (x > x1 || y > y1);
                        break;
                    default:
                        break;
                }
                if (this._root != null && this._root.children != null)
                    this._root = node;
            }
            else
            {
                return;
            }
            this._x0 = x0;
            this._x1 = x1;
            this._y0 = y0;
            this._y1 = y1;
        }

        /// <summary>
        /// 添加节点
        /// </summary>
        /// <param name="x">节点x坐标</param>
        /// <param name="y">节点y坐标</param>
        /// <param name="d">节点数据</param>
        void add(double x, double y, ForceNode d)
        {
            if (x == null || y == null)
                return;
            var node = this._root;
            QuadtreeNode parent = node;
            var leaf = new QuadtreeNode
            {
                IsLeaf = true,
                data = d
            };
            double x0 = this._x0, x1 = this._x1, y0 = this._y0, y1 = this._y1;
            double xm;      //四叉树当前区块x轴中点
            double ym;      //四叉树当前区块y轴中点
            double xp = .0;      //数据结点x坐标
            double yp = .0;      //数据结点y坐标
            int right;
            int bottom;
            int i = 0;      //新结点在四叉树子节点列表中的位置
            int j = 0;      //旧结点在四叉树子节点列表中的位置

            //如果根节点为空，初始化根节点为叶结点
            if (node.children == null && node.data == null)
            {
                this._root = leaf;
                return;
            }

            //找出新叶结点应在坐标，为其赋值
            while (node.children != null)    //当未查找至叶子节点时，不断计算
            {
                //判断叶新节点位于四叉树哪一个区块
                xm = (x0 + x1) / 2;
                if (x >= xm)
                {
                    x0 = xm;
                    right = 1;
                }
                else
                {
                    x1 = xm;
                    right = 0;
                }

                ym = (y0 + y1) / 2;
                if (y >= ym)
                {
                    y0 = ym;
                    bottom = 1;
                }
                else
                {
                    y1 = ym;
                    bottom = 0;
                }

                i = bottom << 1 | right;  //新结点在子节点中的排序

                //当前坐标节点为空时，添加叶结点
                parent = node;
                node = node.children[i];
                if (node == null)
                {
                    parent.children[i] = leaf;
                    return;
                }
            }

            if (node != null && node.data != null)
            {
                xp = node.data.x;
                yp = node.data.y;
            }

            //判断新结点是否与已存在的节点的坐标完全一致
            if (x == xp && y == yp)
            {
                if (parent.children != null || parent.data != null)
                {
                    if (parent.children == null)
                        parent.children = new QuadtreeNode[4];
                    parent.children[i] = leaf;
                }
                else
                    this._root = leaf;
                return;
            }

            //切割叶结点，直到新旧节点并存
            var rootNode = parent;
            if (rootNode.data == null)
            {
                int dfe = 0;
            }
            do
            {
                if (parent.children != null)
                {
                    parent.children[i] = new QuadtreeNode();
                    parent = parent.children[i];
                    parent.children = new QuadtreeNode[4];
                }
                else
                {
                    this._root.children = new QuadtreeNode[4];
                    //parent = this._root;
                }
                    

                //判断叶新节点位于四叉树哪一个区块
                xm = (x0 + x1) / 2;
                if (x >= xm)
                {
                    x0 = xm;
                    right = 1;
                }
                else
                {
                    x1 = xm;
                    right = 0;
                }

                ym = (y0 + y1) / 2;
                if (y >= ym)
                {
                    y0 = ym;
                    bottom = 1;
                }
                else
                {
                    y1 = ym;
                    bottom = 0;
                }

                i = bottom << 1 | right;  //新结点在子节点中的排序
                bool isOldY = yp >= ym;
                bool isOldX = xp >= xm;
                if (isOldY)
                    if (isOldX)
                        j = 1 << 1 | 1;
                    else
                        j = 1 << 1 | 0;
                else
                    if (isOldX)
                        j = 0 << 1 | 1;
                    else
                        j = 0 << 1 | 0;

            } while (i == j);
            //清除原有数据，并赋新值
            var oldNode = new QuadtreeNode
            {
                data = node.data,
                IsLeaf = true
            };
            rootNode.data = null;
            rootNode.IsLeaf = false;
            parent.children[j] = oldNode;
            parent.children[i] = leaf;
        }


        public void visitAfter(CallBackHandler5 callback)
        {
            var quads = new Stack<QuadtreeVisit>();
            var next = new Stack<QuadtreeVisit>();
            QuadtreeVisit q = null;
            if (this._root != null)
            {
                quads.Push(quad(this._root, this._x0, this._y0, this._x1, this._y1));
            }
            while (quads.Count > 0)
            {
                q = quads.Pop();
                var node = q.node;
                //判断是否已经到了底层叶结点
                if (node.children != null)
                {
                    double x0 = q.x0, y0 = q.y0, x1 = q.x1, y1 = q.y1, xm = (x0 + x1) / 2, ym = (y0 + y1) / 2;
                    if (node.children[0] != null)
                        quads.Push(quad(node.children[0], x0, y0, xm, ym));
                    if (node.children[1] != null)
                        quads.Push(quad(node.children[1], xm, y0, x1, ym));
                    if (node.children[2] != null)
                        quads.Push(quad(node.children[2], x0, ym, xm, y1));
                    if (node.children[3] != null)
                        quads.Push(quad(node.children[3], xm, ym, x1, y1));
                }
                next.Push(q);
            }
            while (next.Count > 0)
            {
                q = next.Pop();
                callback(q.node, q.x0, q.y0, q.x1, q.y1);
            }
        }

        public void visit(CallBackHandler3 callback)
        {
            var quads = new Stack<QuadtreeVisit>();
            QuadtreeVisit q;
            var node = this._root;
            double x0, y0, x1, y1;
            if (node != null)
            {
                quads.Push(quad(node, this._x0, this._y0, this._x1, this._y1));
            }
            while (quads.Count > 0)
            {
                q = quads.Pop();
                x0 = q.x0; y0 = q.y0; x1 = q.x1; y1 = q.y1;
                bool isSuccess = callback(q.node, q.x0, q.x1);
                node = q.node;
                if (!isSuccess && node.children != null)
                {
                    double xm = (x0 + x1) / 2;
                    double ym = (y0 + y1) / 2;
                    if (node.children[3] != null)
                        quads.Push(quad(node.children[3], xm, ym, x1, y1));
                    if (node.children[2] != null)
                        quads.Push(quad(node.children[2], x0, ym, xm, y1));
                    if (node.children[1] != null)
                        quads.Push(quad(node.children[1], xm, y0, x1, ym));
                    if (node.children[0] != null)
                        quads.Push(quad(node.children[0], x0, y0, xm, ym));
                }
            }
        }

        /// <summary>
        /// 查询初始化
        /// </summary>
        /// <param name="node">初始节点</param>
        /// <param name="x0"></param>
        /// <param name="y0"></param>
        /// <param name="x1"></param>
        /// <param name="y1"></param>
        /// <returns></returns>
        QuadtreeVisit quad(QuadtreeNode node, double x0, double y0, double x1, double y1)
        {
            double x00 = x0, y00 = y0, x10 = x1, y10 = y1;
            QuadtreeVisit visit = new QuadtreeVisit
            {
                node = node,
                x0 = x0,
                x1 = x1,
                y0 = y0,
                y1 = y1
            };
            return visit;
        }
    }
}
